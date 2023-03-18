<?php

return [
    "secret" => "",
    "db" => __dir__.DIRECTORY_SEPARATOR.'store'.DIRECTORY_SEPARATOR.'db.sqlite',
    "values" => [
        'hum' => ["label" => "humidity"], 
        'temp' => ["label" => "temperatur"], 
        'co' => ["label" => "carbon monoxide"], 
        'co2' => ["label" => "carbon dioxide"], 
        'tv' => ["label" => "particle"]
    ]
];