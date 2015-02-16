#!/bin/awk -f

BEGIN {
  n = 0
}

{
  if ($8 ~ /R\+?/) {
    v[n++] = $2
  }    
}

END {
  for (i=0; i<n; i++) {
    for (j=0; j<n-i-1; j++) {
      if (v[j] > v[j+1]) {
        tmp = v[j+1]
        v[j+1] = v[j]
        v[j] = tmp
      }
    }
  }

  for (i=0; i<n; i++) {
    printf "%d ", v[i]
  }
}
