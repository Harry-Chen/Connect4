Workflow RunDLLTest {
	Param($numbers)
	ForEach -Parallel ($i in $numbers){
		$programName = "..\Release\Compete.exe"
		$myDllName = "..\Release\Strategy.dll"
		$opponentDllName = "..\TestCases\" + "$i" + ".dll"
		$resultName = "$i" + ".txt"
		$time = 10
		$command = $programName, $myDllName, $opponentDllName, $resultName, $time -join " "
		Write-Output $command
		Invoke-Expression $command
	}
}

For ($begin = 2;$begin -lt 100;$begin = $begin + 2 * 8) {
	$numberList = ((($begin)..($begin + 2 * 7)) | Where-Object {$_ % 2 -eq 0})
	RunDLLTest($numberList)
}