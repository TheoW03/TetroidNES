function  build {
    $RomList = "--target nes ./TestRomsSource/LDAtest.s -o ./TestRoms/LDATest.nes",
    ":3"
    foreach ($p in $RomList) {
        & "C:\\cc65\\bin\\cl65" @($p -split "\s")
    }
    
}