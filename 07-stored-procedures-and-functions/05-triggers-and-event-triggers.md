# Triggers

```sql
CREATE TRIGGER trg_update_timestamp
  BEFORE UPDATE ON users
  FOR EACH ROW EXECUTE FUNCTION update_timestamp();
```
