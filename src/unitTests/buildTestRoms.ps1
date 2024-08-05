function  build {
    $RomList = "--target nes ./TestRomsSource/LDAtest.s -o ./TestRoms/LDATest.nes",
    "--target nes ./TestRomsSource/JMPTest.s -o ./TestRoms/JMPTest.nes"
    foreach ($p in $RomList) {
        & "C:\\cc65\\bin\\cl65" @($p -split "\s")
    }
    
}