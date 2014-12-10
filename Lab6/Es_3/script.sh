#!/bin/bash
find / -name "*.exe" -size +1024c
find ~ -name *.c -size +100c
find test/ -user neslinesli93 -regex ".*/.*a.*A.*\|.*/.*A.*a.*"
find ~ -mindepth 3 -maxdepth 5 -type f -user neslinesli93 -size +250c -regex ".*/[a-zA-Z0-9][a-zA-Z0-9][a-zA-Z0-9][a-zA-Z0-9]" -exec wc -c -l \{} \;
find / -regex ".*/A[^z]*zzz[^z]*\.[a-zA-Z0-9][a-zA-Z0-9][a-zA-Z0-9]" -exec tar -czvf archivio.tar.gz \{} \;
find / -type d -regex ".*/bin/.*" -exec ls -lh \{} \;
find / -group users ! -user neslinesli93 -name "*.c"
find ~/../ -type f ! -user neslinesli93 -exec chmod -rwx \{} \;
