#!/bin/awk -f

{
  if ($8 ~ /R\+?/)
    print $NF
}
