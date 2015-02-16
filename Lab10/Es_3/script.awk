BEGIN {
  getline
  getline
  for (i=1; i<NF; i++) {
        days[i] = $i
  }
}

# This sucks so much I don't even
{
  if (DAY <= 6 && NR <= 6) {
    new_day = DAY+7
  } else if (DAY >= 21 && NR <= 6) {
    new_day = DAY-7
  } else {
    new_day = DAY
  }

  for (i=1; i<NF; i++) {
    if ($i == new_day) {
      printf "It was a %s", days[i]
    }
  }
}
