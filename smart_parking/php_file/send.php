<?php 
header("Cache-Control: max-age=3440");
//$data = "celsius = ".$_POST["cel"]." and fahrenheit = ".$_POST["fer"].".";
//file_put_contents("data.txt",$data);
//$data = "\ncelsius = ".$_GET['cel'];//." and fahrenheit = ".$_GET["fer"];
/*$my_file = 'data.txt';
$handle = fopen($my_file, 'a');
fwrite($handle, "\ncelsius = 30.22 and fahrenheit = 82.33");
echo "1";*/
//file_put_contents("data.txt","DATA WRITTEN");

/*
if($_SERVER["REQUEST_METHOD"] == "POST"){
	if(empty(($_POST)){
		file_put_contents("data.txt","POST EMPTY");
	}else{
		file_put_contents("data.txt","DATA WRITTEN");
	}	
}else{
	echo "not post.";
}*/
/*$data = "\ncelsius = ".$_GET['cel']." and fahrenheit = ".$_GET["fer"];
$my_file = 'data.txt';
$handle = fopen($my_file, 'a');
fwrite($handle, $data);
echo "SUCCESS";
*/

$data = "\ncentimeter = ".$_GET['cm']." and inch = ".$_GET["inch"];
$my_file = 'data.txt';
$handle = fopen($my_file, 'a');
fwrite($handle, $data);
echo "SUCCESS";
?>