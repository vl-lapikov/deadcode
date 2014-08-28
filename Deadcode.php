<?php

class PHP_CodeCoverage_Driver_Deadcode extends PHP_CodeCoverage_Driver
{
    protected function ensureDriverCanWork()
    {
        return true;
    }

    /**
     * Start collection of code coverage information.
     */
    protected function doStart()
    {
        return true;
    }

    /**
     * Stop collection of code coverage information.
     *
     * @return array
     */
    protected function doStop()
    {
        return array(
            '/work/workspace/c/deadcode/analyze.php' => array(
                7 => 1,
                8 => 1,
                9 => 1,
                15 => 1,
                16 => 1,
                17 => 1,
            ));
    }

    /**
     * @param array $data
     */
    protected function cleanup(array &$data)
    {
        foreach (array_keys($data) as $file) {
            if (isset($data[$file][0])) {
                unset($data[$file][0]);
            }

            if (file_exists($file)) {
                $numLines = $this->getNumberOfLinesInFile($file);

                foreach (array_keys($data[$file]) as $line) {
                    if (isset($data[$file][$line]) && $line > $numLines) {
                        unset($data[$file][$line]);
                    }
                }
            }
        }
    }

    /**
     * @param  string $file
     * @return integer
     */
    protected function getNumberOfLinesInFile($file)
    {
        $buffer = file_get_contents($file);
        $lines  = substr_count($buffer, "\n");

        if (substr($buffer, -1) !== "\n") {
            $lines++;
        }

        return $lines;
    }
}
