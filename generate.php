<?php

$options = getopt("", array("path:", "regex:"));
if (!isset($options['path'])) {
    echo "Please specify --path [directory to scan]" . PHP_EOL;
    exit;
}
//if (!isset($options['regex'])) {
//    echo "Please specify --regex [matching pattern]" . PHP_EOL;
//    exit;
//}
if (!is_array($options['path'])) {
    $options['path'] = array($options['path']);
}

foreach ($options['path'] as $path) {
    $Directory = new RecursiveDirectoryIterator($path);
    $iterator = new RecursiveIteratorIterator($Directory);
    //$regex = new RegexIterator($Iterator, '/' . $options['regex'] . '/i', RecursiveRegexIterator::GET_MATCH);
    foreach ($iterator as $info) {
        if (!in_array($info->getFilename(), array('.', '..')))k {
            echo $info->getPathname() . $info->getFilename() . PHP_EOL;
        }
    }
}


