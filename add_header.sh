#!/bin/bash
# -------------------------------------------------------------
# Script: add_header.sh
# Purpose: Inserts the 42 header into specified C/H files.
# -------------------------------------------------------------

# --- CONFIGURATION (Customize these variables) ---
HEADER_TEMPLATE="./header.txt"
LOGIN="rluis-ya"                               # Your 42 login
EMAIL="${LOGIN}@student.42porto.com"               # Your 42 email

# --- CORE FUNCTION ---
add_header_to_file() {
    local FILE_PATH="$1"
    local FILE_NAME=$(basename "$FILE_PATH")
    local DATE=$(date "+%Y/%m/%d %H:%M:%S")

    # 1. Check if the file is a standard file
    if [ ! -f "$FILE_PATH" ]; then
        echo "Error: Not a regular file or file not found: $FILE_PATH"
        return 1
    fi

    # 2. Check if the file extension is relevant (.c or .h)
    case "$FILE_NAME" in
        *.c|*.h)
            ;;
        *)
            echo "Skipping $FILE_NAME: Not a .c or .h file."
            return 0
            ;;
    esac

    # 3. Check if file already has the header (simple check for the first line comment)
    if head -n 1 "$FILE_PATH" | grep -q "/* \*" ; then
        # echo "Warning: Header already exists in $FILE_NAME. Skipping."
        return 0
    fi

    # 4. Generate Header Content
	HEADER_CONTENT=$(sed \
        -e "s|<filename>|$FILE_NAME|g" \
        -e "s|<login>|$LOGIN|g" \
        -e "s|<42mail>|$EMAIL|g" \
        -e "s|<date>|$DATE|g" \
        "$HEADER_TEMPLATE")

    # 5. Inject Header
    TEMP_FILE=$(mktemp)
    if [ $? -ne 0 ]; then
        echo "Error: Failed to create temporary file."
        return 1
    fi
    
    echo "$HEADER_CONTENT" > "$TEMP_FILE"
    cat "$FILE_PATH" >> "$TEMP_FILE"
    mv "$TEMP_FILE" "$FILE_PATH"

    echo "✅ Added header to $FILE_NAME"
}

# --- MAIN EXECUTION ---
if [ "$#" -eq 0 ]; then
    echo "Usage: $0 <path/to/file1> [<path/to/file2> ...]"
    exit 1
fi

for FILE in "$@"; do
    add_header_to_file "$FILE"
done
exit 0
