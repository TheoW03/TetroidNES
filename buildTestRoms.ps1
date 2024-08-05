function  build {
    $RomList = "--target nes ./src/unitTests/TestRomsSource/LDAtest.s -o ./src/unitTests/TestRoms/LDATest.nes",
    "--target nes ./src/unitTests/TestRomsSource/JMPTest.s -o ./src/unitTests/TestRoms/JMPTest.nes"
    foreach ($p in $RomList) {
        & "C:\\cc65\\bin\\cl65" @($p -split "\s")
    }
    
}