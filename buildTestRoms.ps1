function  build {
    $RomList = "nya~",
    "./src/unitTests/TestRomsSource/LDAtest.s -o ./src/unitTests/TestRoms/LDATest.nes",
    "./src/unitTests/TestRomsSource/JMPTest.s -o ./src/unitTests/TestRoms/JMPTest.nes",
    "./src/unitTests/TestRomsSource/TestAsl.s -o ./src/unitTests/TestRoms/TestAsl.nes"

    echo ""

    foreach ($p in $RomList) {
        $args = @($p -split "\s")
        & "cl65" "--target" "nes" @args
        if ($?) {
            Write-Output ("Built {0} successfully" -f $args[2])

        }
        else {
            Write-Output ("Built {0} unsuccessfully" -f $args[2])

        }
        Write-Output ""
        
    }
    
}