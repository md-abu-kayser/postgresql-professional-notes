# Common Errors & Solutions

- `duplicate key value violates unique constraint` – handle with `ON CONFLICT`.
- `could not serialize access` – retry transaction or adjust isolation level.
- `too many connections` – configure connection pooler.
