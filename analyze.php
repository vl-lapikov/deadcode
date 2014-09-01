<?php

class Deadcode_AliveFile
{
    const INTERNAL = '___internal___';
    const ALIVE = 'alive';
    const START_LINE = 'startLine';
    const END_LINE = 'endLine';
    private $_classes = array();
    private $_filename;

    public function __construct($filename)
    {
        $this->_filename = $filename;
        $this->parseFunctions();
    }

    public function getFilename()
    {
        return $this->_filename;
    }

    public function setAliveFunction($class, $name)
    {
        if ($class) {
            $this->setClass($class);
        } else {
            $class = self::INTERNAL;
        }
        $this->_classes[$class][$name][self::ALIVE] = true;
    }

    public function getCodeCoverage()
    {
        $result = array();
        foreach ($this->_classes as $class)
        {
            foreach ($class as $function) {
                for ($i = $function[self::START_LINE]; $i <= $function[self::END_LINE]; $i++) {
                    $result[$this->_filename][$i] = $function[self::ALIVE];
                }
            }
        }
        return $result;
    }

    private function setClass($class)
    {
        if (!isset($this->_classes[$class])) {
            $this->_classes[$class] = array();
        }
        return $this;
    }

    private function parseFunctions()
    {
        $stream = new PHP_Token_Stream($this->_filename);
        foreach ($stream->getFunctions() as $name => $function) {
            $this->_classes[self::INTERNAL][$name] = array(
                self::START_LINE => $function[self::START_LINE],
                self::END_LINE => $function[self::END_LINE],
                self::ALIVE => false,
            );
        }
        foreach ($stream->getClasses() as $className => $class) {
            foreach ($class['methods'] as $name => $function) {
                $this->_classes[$className][$name] = array(
                    self::START_LINE => $function[self::START_LINE],
                    self::END_LINE => $function[self::END_LINE],
                    self::ALIVE => false,
                );
            }
        }
    }
}

function getNextAliveFile($filename) {
    static $handler, $passed;
    if (!$handler) {
        $handler = fopen($filename, 'r');
    }
    $newFile = false;
    while (($line = fgets($handler)) !== false) {
        $function = explode(':', $line);
        if (!isset($function[0]) || !isset($function[2])) {
            break;
        }
        $filename = trim($function[0]);
        $class = trim($function[1]);
        $function = trim($function[2]);
        if (!$newFile && !isset($passed[$filename])) {
            $newFile = new Deadcode_AliveFile($filename);
        }
        if ($newFile->getFilename() == $filename) {
            $newFile->setAliveFunction($class, $function);
        }
    }
    return $newFile;
}

require_once 'vendor/autoload.php';
require_once 'Deadcode.php';

$driver = new PHP_CodeCoverage_Driver_Deadcode(new PHP_CodeCoverage_Filter(), new PHP_CodeCoverage_Parser());

while ($file = getNextAliveFile('deadcode.dump')) {
    $coverage = new PHP_CodeCoverage($driver);
    $coverage->append($file->getCodeCoverage(), $file->getFilename());
    $writer = new PHP_CodeCoverage_Report_HTML();
    $writer->process($coverage, '/tmp/coverage');
}
exit;
$file = '/work/workspace/php/contract-module/src/application/controllers/ContractController.php';

$function_happens = false;
$function_name_happens = false;
$function_body_started = false;
$is_calculate_next_line = false;
$previous_line_number = 0;
$bracket = 0;
$functions = array();
$current = 0;
foreach(token_get_all(file_get_contents($file)) as $token) {
    $output = '';
    if (is_array($token)) {
        switch ($token[0]) {
            case T_FUNCTION:
                $output = "Function: ";
                $function_happens = true;
                $function_name_happens = false;
                $functions[$current] = array();
                break;
            case T_STRING:
                if ($function_happens && (!$function_name_happens)) {
                    $functions[$current]['name'] = $token[1];
                    $output = $token[1] . PHP_EOL;
                    $function_name_happens = true;
                }
                break;
        }
        if ($token[0] != T_WHITESPACE) {
	        $previous_line_number = $token[2];
	        if ($is_calculate_next_line) {
	            $functions[$current]['start'] = $token[2];
	            $is_calculate_next_line = false;
	        }
        }
    } else {
        switch ($token) {
            case '{':
                if ($function_happens) {
                    $bracket = 1;
                    $functions[$current]['start'] = '{';
                    $function_happens = false;
                    $function_body_started = true;
                    $is_calculate_next_line = true;
                } else {
                    $bracket++;
                }
                break;
            case '}':
                if (!$function_body_started) {
                    continue;
                }
                $bracket--;
                if ($bracket < 0) {
                    echo 'Syntax error! Bracket = -1';
                }
                if ($bracket == 0) {
                    $functions[$current]['end'] = $previous_line_number;
                    $function_body_started = false;
                    $current++;
                }
                break;
        }
    }
}
print_r($functions);

