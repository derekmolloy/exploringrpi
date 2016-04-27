<?php $temperature = shell_exec('/usr/local/bin/dht'); ?>
<?php $cpu_temp = (float) file_get_contents('/sys/class/thermal/thermal_zone0/temp'); ?>
<html><head><title>RPi PHP Test</title></head>
 <body>
 <h1>Hello from the Raspberry Pi</h1>
 <div>Your IP address is: <?php echo $_SERVER['REMOTE_ADDR']; ?></div>
 <div><?php echo $temperature ?></div>
 <div><h3>The CPU temperature is: <?php echo $cpu_temp/1000 ?>°C</h3></div>
 </body>
</html>

