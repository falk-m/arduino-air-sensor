<?php

class Database 
{
    private PDO $pdo;

    public function __construct(string $file)
    {
        if(!file_exists($file)){
            $this->setup($file);
        }
        $this->pdo = new PDO("sqlite:{$file}");
    }

    private function setup(string $file) 
    {
        try {
            $this->pdo = new PDO("sqlite:{$file}");
        } catch (\Throwable $th) {
            $dir = dirname($file);
            throw new Exception("can not create file \"{$file}\". Plead check the permissions on the folder \"{$dir}\"");
        }
        
        $command = 'CREATE TABLE IF NOT EXISTS `values` (
                        `date` INTEGER,
                        `key` VARCHAR(5),
                        `value` FLOAT,
                        PRIMARY KEY(date,key)
                      )';

        $this->pdo->exec($command);
    }

    public function insertValue(string $key, float $value)
    {
        $sql = 'INSERT INTO `values` (`date`, `key`, `value`) VALUES(:date, :key, :value)';
        $stmt = $this->pdo->prepare($sql);
        $stmt->bindValue(':date', time());
        $stmt->bindValue(':key', $key);
        $stmt->bindValue(':value', $value);
        $stmt->execute();
    }

    public function getValues(string $key, int $from = 0, int $to =0)
    {
        if($to == 0){ $to = time(); }
        $stmt = $this->pdo->prepare('SELECT `date`, 
                                            `value`
                                     FROM   `values`
                                     WHERE  `key` = :key
                                        AND `date` >= :from
                                        AND `date` <= :to');

        $stmt->execute([':key' => $key, 'from' => $from, 'to' => $to]);

        $res = [];
        while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
            $res[] = $row;
        }

        return $res;
    }
}