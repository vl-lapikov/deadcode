<?php

class ExampleClass 
{
    public function exampleMethod()
    {
        echo __FUNCTION__ . PHP_EOL;
        echo "Example inner code" . PHP_EOL;
        return true;
    }

    public function exampleDeadMethod()
    {
        echo __FUNCTION__ . PHP_EOL;
        echo "Example dead inner code" . PHP_EOL;
        return true;
    }
}

function exampleFunction()
{
    echo __FUNCTION__ . PHP_EOL;
    echo "Example inner code" . PHP_EOL;
    return true;
}

function exampleDeadFunction()
{
    echo __FUNCTION__ . PHP_EOL;
    echo "Example deadd inner code" . PHP_EOL;
    return true;
}

echo "Example out code";

$example = new ExampleClass();
$example->exampleMethod();
exampleMethod();

