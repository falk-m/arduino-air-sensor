<?php

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

require_once('src'.DIRECTORY_SEPARATOR.'Database.php');
$config = require('config.php');
$db = new Database($config["db"]);

if(!empty($_GET["secret"])){
    if($_GET["secret"] != $config["secret"]){
        echo 0;
        exit;
    }
    unset($_GET["secret"]);
    foreach($config["values"] as $key => $settings){
        $db->insertValue($key, floatval($_GET[$key] ?? "0"));
    }

    echo 1;
    exit;
}

echo 0;