# Covering Indexes

```sql
CREATE INDEX idx_users_name_inc_email ON users (name) INCLUDE (email);
```
