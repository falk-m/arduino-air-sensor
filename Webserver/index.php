<?php

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

require_once('src'.DIRECTORY_SEPARATOR.'Database.php');
$config = require('config.php');
$db = new Database($config["db"]);

$selectedKey = $_GET["value"] ?? "temp";
$from = intval($_GET["from"] ?? (time() - (60 * 60 * 24)));
$to = $from + (60 * 60 * 24);

$selectedSettings = $config["values"][$selectedKey];

$data = $db->getValues($selectedKey, $from, $to);
//print_r($data);
?><!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Arduino Senors</title>

    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@3.3.7/dist/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">
  </head>
  <body>
    <div class="container-fluid">
        <h1>Sensor Data</h1>

        <nav aria-label="Page navigation">
            <ul class="pagination">
                <li>
                <a href="?<?php echo http_build_query(["value" => $selectedKey, "from" => $from - (60 * 60 * 24)]); ?>" aria-label="Previous">
                    <span aria-hidden="true">&laquo;</span>
                </a>
                </li>
                <li><a href="#"><?php echo "from ".date("d.m.Y H:i", $from)." to ".date("d.m.Y H:i:s", $to); ?></a></li>
                <li><a href="?<?php echo http_build_query(["value" => $selectedKey]); ?>"><i class="glyphicon glyphicon-refresh"></i> refresh</a></li>
                <li>
                <a href="?<?php echo http_build_query(["value" => $selectedKey, "from" => $from + (60 * 60 * 24)]); ?>" aria-label="Next">
                    <span aria-hidden="true">&raquo;</span>
                </a>
                </li>
            </ul>
        </nav>

        <ul class="nav nav-tabs">
            <?php foreach($config["values"] as $valueKey => $valueSettings){ ?>
                <li role="presentation" class="<?php echo $valueKey == $selectedKey ? 'active' : ''; ?>"><a href="?<?php echo http_build_query(["value" => $valueKey, "from" => $from]); ?>"><?php echo $valueSettings["label"]; ?></a></li>
            <?php } ?>
        </ul>

        <div style="max-height: 80vh;">
            <canvas id="myChart"></canvas>
        </div>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script>
        const data = <?php echo json_encode([
            "labels" => array_map(function($d){ return date("d.m. H:i",$d["date"]);}, $data),
            "datasets" => [[
                "label" => $selectedSettings["label"],
                "data" => array_map(function($d){ return $d["value"];}, $data),
                "borderWidth" => 1
            ]]
        ]);  ?>
    </script>
    <script>
        const ctx = document.getElementById('myChart');

        new Chart(ctx, {
            type: 'line',
            data: data,
            options: {
                scales: {
                y: {
                    beginAtZero: true
                }
                }
            }
        });
    </script>
  </body>
</html>