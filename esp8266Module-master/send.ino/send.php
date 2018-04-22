<?php 
$data = "celsius = ".$_POST["cel"]." and fahrenheit = ".$_POST["fer"].".";
file_put_contents("data.txt",$data);
?>