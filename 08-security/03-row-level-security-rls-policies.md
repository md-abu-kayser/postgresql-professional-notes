# Row‑Level Security (RLS)

```sql
ALTER TABLE users ENABLE ROW LEVEL SECURITY;
CREATE POLICY user_policy ON users USING (id = current_setting('app.current_user_id')::int);
```
