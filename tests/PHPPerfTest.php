<?
$mtime = microtime(); 
$mtime = explode(" ",$mtime); 
$mtime = $mtime[1] + $mtime[0]; 
$starttime = $mtime; 

$NI = 0;
#$AArray = new Array();

for ($NI = 0; $NI < 10000; ++$NI)
{
	if ($NI % 2 == 0)  { $AArray[$NI] = $NI; }
	else
	{
		$NJ = 0;
		for ($NJ = 0; $NJ < 100; ++$NJ)
		{
			$SKey = sprintf("_%d_%d_", $NI, $NJ);
			$AArray[$NI][$SKey][10] = $NI;
		}
	}

}

$mtime = microtime(); 
$mtime = explode(" ",$mtime); 
$mtime = $mtime[1] + $mtime[0]; 
$endtime = $mtime; 
$elapsed = ($endtime - $starttime); 

print("Time: " . $elapsed . "\n")
?>
