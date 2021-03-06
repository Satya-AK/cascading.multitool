# Copyright (c) 2011 Concurrent, Inc.

module_depends log hadoop jar help doc

route_default mt_run
route_before mt_run mt_run_error_argless mt_hadoop mt_jar

mt_run_error_argless ()
{
  if [ $# -eq 0 ]
  then
    mt_help
    echo
  fi
}

mt_run ()
{
  if [ $# -eq 0 ]
  then
    module_exit "No arguments specified"
  else
    # the -r switch on read makes sure, that we do not lose \ characters
    $HADOOP_BIN jar $mt_jar_path $@ 2>&1 | while read -r line; do
      if echo $line | grep INFO > /dev/null; then
        info $line
      elif echo $line | grep WARN > /dev/null; then
        warn $line
      elif echo $line | grep ERROR > /dev/null; then
        error $line
      elif echo $line | grep Exception > /dev/null; then
        stacktrace $line
      elif echo $line | grep "^error: " > /dev/null; then
        mt_doc_argument_error $line
        module_exit
      elif [ -n "$mt_log_verbose" ]; then
        log $line
      fi
    done
  fi
}
