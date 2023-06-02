# testall.ps1 - テストスクリプト
#
# ISC License
# 
# Copyright 2023 Kitajima Akira
# 
# Permission to use, copy, modify, and/or distribute this software for any purpose 
# with or without fee is hereby granted, provided that the above copyright notice
# and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, 
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
# OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
# TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
# THIS SOFTWARE.

# スクリプトの起点ディレクトリ
$scriptRoot = Split-Path -Path $PSCommandPath

# 構成ファイルtestconfig.jsonを読み込む。
$testConfigFile = "testconfig.json"
if (Test-Path $testConfigFile) {
    $scriptRoot = (Get-Item ".").FullName
} else {
    $testConfigFile = Join-Path $scriptRoot $testConfigFile
}

if (!(Test-Path $testConfigFile)) {
    Write-Error "configuration file $testConfigFile not found."
    Exit-PSSession
}
$testConfig = (Get-Content $testConfigFile | ConvertFrom-Json)

$testCommand = Join-Path $scriptRoot $testConfig.testCommand
if (!(Test-Path $testCommand)) {
    # 実行ファイルがなければそのままコマンドとして実行する。
    $testCommand = $testConfig.testCommand
}

# 作業ディレクトリ
$workDirectoryName = $testConfig.workDirectoryName
# 期待値ディレクトリ
$expectedDirectoryName = $testConfig.expectedDirectoryName

# 作業ディレクトリ・期待値ディレクトリを用意する。
$workDirectory = Join-Path $scriptRoot $workDirectoryName
$expectedDirectory = Join-Path $scriptRoot $expectedDirectoryName

if (!(Test-Path $workDirectory)) {
    $null = New-Item $workDirectory -ItemType Directory 
}
if (!(Test-Path $expectedDirectory)) {
    $null = New-Item $expectedDirectory -ItemType Directory
}

# ログファイル
$logFileName = $testConfig.logFileName

# テストを実施する。
function ImportFile {
    Param(
        $source,
        $path
    )
    if (Test-Path $path) {
        # ソースもテスト入力もある場合
        $sourceWriteTime = (Get-Item $source).LastWriteTime
        $inputWriteTime  = (Get-Item $path).LastWriteTime
        if ($inputWriteTime -lt $sourceWriteTime) {
            Copy-Item $source $path
            Write-Host "import: $source"
        }

    } else {
        # ソースがあってテスト入力がない場合 (インポート)
        Copy-Item $source $path
        Write-Host "import: $source"
    }
}

# テスト結果の情報
$stat = @{
    ok = 0
    ng = 0
    add = 0
    ngList = @()
    ngLog = ""
}

$beginTotalTime = Get-Date
Write-Host "Begin test: $beginTotalTime"

# テストディレクトリ($WorkDirectory)に移動
Push-Location -Path $WorkDirectory

:singleTest foreach ($case in $testConfig.testCase) {
    Write-Host -NoNewline "`ncase: "
    Write-Host $case.inputFiles -ForegroundColor yellow

    # 入力の準備
    $inputFile = $null
    $sourceDirectory = $null
    if ($case.sourceDirectory) {
        $sourceDirectory = Join-Path $scriptRoot $case.sourceDirectory
    } else {
        $sourceDirectory = Join-Path $scriptRoot $testConfig.defaultSourceDirectory
    }
    foreach ($i in $case.inputFiles) {
        if (!$inputFile) {
            $inputFile = $i
        }
        $src = Join-Path $sourceDirectory $i
        $dest = Join-Path $workDirectory $i
        if (Test-Path $src) {
            ImportFile $src $dest
        } elseif (!(Test-Path $dest)) {
            Write-Host -noNewLine "file not found: "
            Write-Host $i -ForegroundColor Red
            $stat.ng += 1
            $stat.ngLog += "${i}: file not found`n`n"
            continue singleTest
        }
    }

    # 期待値の準備
    $outputFile = $null
    foreach ($i in $case.outputFiles) {
        if (!$outputFile) {
            $outputFile = $i
        }
        $src = Join-Path $sourceDirectory $i
        $dest = Join-Path $expectedDirectory $i
        if (!(Test-Path $dest) -and (Test-Path $src)) {
            ImportFile $src $dest
        }
    }

    # 出力ファイルのクリーンアップ
    foreach ($out in $case.outputFiles) {
        if (Test-Path $out) {
            Remove-Item $out
        }
    }

    if ($case.arguments) {
        $arguments = $case.arguments
    } else {
        $arguments = $inputFile
    }

    # テストの実行
    $beginTime = Get-Date
    # $commandLine = $testCommand + " $arguments"
    $log = powershell $testCommand $arguments 2>&1
    $endTime = Get-Date
    Write-Host "done ($(($endTime - $beginTime).TotalSeconds) sec)."

    # 結果の確認
    foreach ($out in $case.outputFiles) {
        $expectedOutput = Join-Path $expectedDirectory $out
        Write-Host -NoNewline "${out}: "

        if (Test-Path $out) {
            # 出力ファイルが生成された場合
            if (!(Test-Path $expectedOutput)) {
                # 期待値がない場合
                Move-Item $out $expectedDirectory
                Write-Host "Added" -ForegroundColor Cyan
                $stat.add += 1
                continue
            }
            $result = Compare-Object -ReferenceObject @(Get-Content -Path $expectedOutput) -DifferenceObject @(Get-Content -Path $out)
            if ($result) {
                Write-Host "NG" -ForegroundColor Red
                $stat.ng += 1
                $stat.ngList += $out
                $stat.ngLog += "* $out`n$log`n`n"
                code -diff $out $expectedOutput
            } else {
                Write-Host "OK" -ForegroundColor Green
                $stat.ok += 1
            }
        } else {
            # 出力ファイルが生成されなかった場合
            Write-Host "NG" -ForegroundColor Red
            Write-Host -NoNewline "file not generated: "
            WRite-Host $out -ForegroundColor Red
            $stat.ng += 1
            $stat.ngList += $out
            $stat.ngLog += "* $out`n$log`n`n"
        }
    }
}

Pop-Location

$endTotalTime = Get-Date
Write-Host "`nEnd test: $endTotalTime ($(($endTotalTime - $beginTotalTime).TotalSeconds) sec.)"
Write-Host "`n* Summary:"
Write-Host "OK: $($stat.ok), NG: $($stat.ng), Added: $($stat.add)"
if ($stat.ng) {
    Write-Host "NG files: $($stat.ngList)"
    Set-Content $logFileName $stat.ngLog
    $path = (Get-ItemProperty -Path $logFileName).FullName
    Write-Host "See $path for detail."
}
