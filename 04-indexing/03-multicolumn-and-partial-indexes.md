# Multicolumn & Partial Indexes

```sql
CREATE INDEX idx_name_age ON users (name, age);
CREATE INDEX idx_active_users ON users (id) WHERE active = true;
```
