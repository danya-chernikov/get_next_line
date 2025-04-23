#!/bin/bash

declare -r OUTPUT_FILE="Linux sources"

> "$OUTPUT_FILE"

find . -type f \( -name '*.c' -o -name '*.h' -o -name '*.S' \) -print0 | while IFS= read -r -d '' file; do
	echo "===== FILE: $file =====" >> "$OUTPUT_FILE"
	cat "$file" >> "$OUTPUT_FILE"
	echo "" >> "$OUTPUT_FILE"
done
exit 0
