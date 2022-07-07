param(
    [Parameter(Mandatory=$False, ValueFromPipeline, ValueFromPipeLineByPropertyName, Position=0)]
    [Alias("A")]
    [String]
    $Action = "debug-test",

    [Parameter(Mandatory=$False, ValueFromPipeline, ValueFromPipeLineByPropertyName, Position=1)]
    [Alias("T")]
    $Target = "ALL_BUILD",

    [Parameter(Mandatory=$False, ValueFromPipeline, ValueFromPipeLineByPropertyName, Position=2)]
    [Alias("S")]
    $Settings = "./settings.json"
)

# Test if specified settings file is present
# It is expected to be in the root of the
# source directory, and in the same directory
# as this script
$invocation = Split-Path ((Get-Variable MyInvocation -Scope 1).Value.MyCommand)
$Settings = "$invocation/settings.json" -replace "\\", "/"
if (-Not (Test-Path $Settings)) {
    Write-Host -ForegroundColor Red "Could not find the specified settings file at location: $Settings`n"
    Write-Host -ForegroundColor Red "Please specify the location for the settings file with the -Settings or -S flags"
    Write-Host -ForegroundColor Red "or provide a settings.json file in the same directory as this build script"
    Exit
}

# Parse the specified settings file
$user_config = Get-Content -Raw $Settings | ConvertFrom-Json
$raw_paths = @{
    build = $user_config.global.build_path
    test = $user_config.global.test_path
}

# Find and replace path macros
$paths = @{}
$tokens = @{
    WORKSPACE = $invocation
}
foreach($p in $raw_paths.keys) {
    foreach($t in $tokens.keys) {
        $paths[$p] = $raw_paths[$p] -replace "\$\{$t\}", $tokens[$t]
    }

    $paths[$p] = $paths[$p] -replace "\\", "/"
}

# Configure procedure
if ($Action -match "cfg") {
    $cmd = "cmake"

    # Concat flags
    $flags = $user_config.cmake.win32.configure.flags
    foreach($f in $flags) {
        $cmd += " $f"
    }

    # Concat definitions
    $defs = $user_config.cmake.win32.configure.definitions
    foreach ($d in $defs) {
        $n = $d.name; $t = $d.type; $v = $d.value;
        $cmd += " -D${n}:${t}=${v}"
    }

    # Concat toolchain
    $tools = $user_config.cmake.win32.configure.toolset
    foreach($t in $tools) {
        $n = $t.name; $v = $t.value;
        $cmd += " -T ${n}=${v}"
    }

    # Concat generator
    $gen = $user_config.cmake.win32.configure.generator
    if ($gen -ne "default") {
        $cmd += " -G $gen"
    }

    # Concat architecture
    $arch = $user_config.cmake.win32.configure.arch
    $cmd += " -A $arch"

    # Concat build & source paths
    $build_path = $paths.build
    $cmd += " -B ${build_path}"
    $cmd += " -S $invocation"

    Write-Host -ForegroundColor Yellow "Executing expression: $cmd"
    Invoke-Expression $cmd
}

# Build procedure
if ($Action -match "release" -or $Action -match "debug") {
    $cmd = "cmake"
    $build_type = if ($Action -match "release") { "build_release" } else { "build_debug" }

    # Concat build path
    $build_path = Resolve-Path $paths.build
    $cmd += " --build ${build_path}"

    # Concat build config
    $cfg = if ($Action -match "release") { "Release" } else { "Debug" }
    $cmd += " --config $cfg"

    # Concat build target
    $cmd += " --target $Target"

    # Concat flags
    $flags = $user_config.cmake.win32.$build_type.flags
    foreach($f in $flags) {
        $n = $f.name; $v = $f.value;
        $cmd += " $n $v"
    }

    # Concat bikeshedder
    $cmd += " --"

    Write-Host -ForegroundColor Yellow "Executing expression: $cmd"
    Invoke-Expression $cmd
}

# Test procedure
if ($Action -match "test") {
    $cmd = "ctest -j 14 -T test --output-on-failure"
    $test_path = Resolve-Path $paths.test
    $cwd = Resolve-Path "."

    $cfg = if ($Action -match "release") { "Release" } else { "Debug" }
    $cmd += " -C $cfg"

    $expr = "Set-Location $test_path && $cmd ; Set-Location $cwd"
    Write-Host -ForegroundColor Yellow "Executing expression: $cmd"
    Invoke-Expression $expr
}
