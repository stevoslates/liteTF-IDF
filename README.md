# liteTF-IDF

A fast, lightweight C++ TF-IDF engine with REPL-style querying and folder ingestion.

## What It Does

`liteTF-IDF` is a command-line tool that:
- Ingests plain `.txt` files **extremely fast**
- Tokenizes and indexes them using **log-TF + smoothed IDF**
- Supports **cosine similarity-based search**
- Runs in an interactive **REPL loop** so you can keep querying
- Supports **folder ingestion**, **top-K filtering**, and **document preview**
- Includes a Python script to auto-scrape **random Wikipedia articles** into `.txt` files for testing

## Usage 

### Build the Project

```bash
make
```

### Ingest `.txt` Files

```bash
./main -f file1.txt file2.txt
```

```bash
./main -F ./textdata/
```

### Run REPL Queries
Once documents are ingested, the program enters REPL mode:
```text
Please Enter Query:
climate change policy

[0.83] - textdata/article_climate.txt
[0.52] - textdata/policy_notes.txt
```
Type ```exit``` to quit the loop.

### Available Flags


| Flag         | Description                                                  |
|--------------|--------------------------------------------------------------|
| `-f`         | Ingest a list of individual `.txt` files                     |
| `-F`         | Ingest all `.txt` files from the specified folder            |
| `-K <number>`| Return only the top K scoring documents                      |
| `-D`         | Display the contents of matched documents                    |
| `--help`     | Show usage instructions and exit                             |
