import wikipedia
import os
import re
import unidecode

# Create output folder
os.makedirs("wikipedia_articles", exist_ok=True)

def sanitize_filename(s):
    s = unidecode.unidecode(s)  # Remove accents
    s = re.sub(r'[^\w\-_. ]', '', s)  # Remove invalid filename characters
    s = s.replace(' ', '_')  # Replace spaces
    return s

def save_article(title):
    try:
        content = wikipedia.page(title).content
        url = wikipedia.page(title).url
        filename = sanitize_filename(url) + ".txt"
        filepath = os.path.join("wikipedia_articles", filename)
        with open(filepath, "w", encoding="utf-8") as f:
            f.write(f"URL: {url}\n\n")
            f.write(content)
        print(f"Saved: {title}")
    except Exception as e:
        print(f"Skipped: {title} ({e})")

def main(n=100):
    seen = set()
    while len(seen) < n:
        title = wikipedia.random()
        if title not in seen:
            seen.add(title)
            save_article(title)

if __name__ == "__main__":
    main(100)
