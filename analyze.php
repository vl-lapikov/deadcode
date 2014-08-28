<?php

require_once 'vendor/autoload.php';
require_once 'Deadcode.php';

$driver = new PHP_CodeCoverage_Driver_Deadcode(new PHP_CodeCoverage_Filter(), new PHP_CodeCoverage_Parser());

$coverage = new PHP_CodeCoverage($driver);
$coverage->start('example');
$coverage->stop();

$writer = new PHP_CodeCoverage_Report_HTML();
$writer->process($coverage, '/tmp/coverage');
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

