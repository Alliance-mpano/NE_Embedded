<?php
    if(!empty($_REQUEST["device"])){
        include_once("DataProcessMySQL.php");
        $obj = new DataProcessMySQL();
        $obj->insertData(
            $_REQUEST["device"], 
            $_REQUEST["temperature"], 
            $_REQUEST["humidity"]
            );
    }
    else{
        print "Empty Data!";
    }
?>