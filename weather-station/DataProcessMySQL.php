<?php
include_once "DatabaseManager.php";
class DataProcessMySQL extends DatabaseManager{
    private $WEATHER_TBL = "";
    public function __construct($tbl="rca_weather"){
        $this->WEATHER_TBL = $tbl;
        #Start by creating a table in the database if it doesn't exist yet.
        $sql = "CREATE TABLE IF NOT EXISTS ".$this->WEATHER_TBL." (
            id int(11) PRIMARY KEY AUTO_INCREMENT NOT NULL,
            device varchar(100) NOT NULL,
            temperature varchar(10) NOT NULL,
            humidity varchar(10) NOT NULL,
            timestamp varchar(50) NOT NULL) 
            ENGINE=InnoDB DEFAULT CHARSET=latin1";
            
        $q = parent::__construct()->prepare($sql);
        $q->execute();
    }
    public function getTimestamp($offsetParam){
        #Offset is the number of hours counted from GMT.
        $offsetToSeconds=$offsetParam*60*60; #converting offset to seconds
        $dateFormat="Y-m-d H:i:s";
        return gmdate($dateFormat, time()+$offsetToSeconds);
    } 
    
    public function insertData($device,$temperature, $humidity){
        $sql ="INSERT INTO ".$this->WEATHER_TBL."(
                device,
                temperature, 
                humidity, 
                timestamp
            ) VALUES (
                :device,
                :temperature, 
                :humidity,  
                :timestamp
            )";
        
        $q = parent::__construct()->prepare($sql);
        $q->bindValue(":device", $device); 
        $q->bindValue(":temperature", $temperature); 
        $q->bindValue(":humidity", $humidity); 
        $q->bindValue(":timestamp", $this->getTimestamp(2));
        if($q->execute()){
            print "Success";
        }
        else{
            print "Failure";
        }
    }
    
    private function readData(){
        $sql ="SELECT * FROM ".$this->WEATHER_TBL;
        
        $q = parent::__construct()->prepare($sql);
        $q->execute();
        return $q;
    }    
    public function presentDataFromDB(){
        print "<style>";
        print "
            table { 
                border-spacing: 10px;
                border-collapse: collapse;
            }
            td,th{
                border: 1px solid #AAA;
                padding: 5px;
                text-align: left;
            }
        ";
        print "</style>";
        
        print "<table>";
            print "<tbody>";
                print "<tr>";
                    print "<th>Timestamp</th>";
                    print "<th>Device</th>";
                    print "<th>Temperature</th>";
                    print "<th>Humidity</th>";
                print "</tr>";
                #Parse the content to get line after line
                foreach($this->readData()->fetchAll() as $row):
                    if(!empty($row)){
                        $timestamp=$row["timestamp"];
                        $device=$row["device"];
                        $temperature = $row["temperature"]." <sup>o</sup>C"; 
                        $humidity=$row["humidity"];
                        print "<tr>";
                            print "<td>".$timestamp."</td>";
                            print "<td>".$device."</td>";
                            print "<td>".$temperature."</td>";
                            print "<td>".$humidity."</td>";
                        print "</tr>";
                    }
                endforeach;
            print "</tbody>";
        print "</table>";
    }
    public function createAPI(){
        $response = array();
        $sql = "
        SELECT 
        device,
        temperature,
        humidity,
        timestamp
        FROM ".$this->WEATHER_TBL;
        $q = parent::__construct()->prepare($sql);
        $q->execute();
        if($q->rowCount()>0){
            while($row=$q->fetch(PDO::FETCH_ASSOC)){
                extract($row);
                $response[] = $row;
            }
            print "{\"data\":".json_encode($response)."}";
        }
        else{
            echo "{\"data\":[
                {\"device\":\"null\",
                \"temperature\":\"null\",
                \"humidity\":\"null\", 
                \"timestamp\":\"null\"}]}";
        }
    } #End Create API    
}
?>