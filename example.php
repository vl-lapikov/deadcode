<?php

class ExampleClass 
{
    public function exampleMethod()
    {
        echo __FUNCTION__ . PHP_EOL;
        echo "Example inner code" . PHP_EOL;
        return true;
    }
}

function exampleFunction()
{
    echo __FUNCTION__ . PHP_EOL;
    echo "Example inner code" . PHP_EOL;
    return true;
}

echo "Example out code";

$example = new ExampleClass();
$example->exampleMethod();
exampleMethod();

