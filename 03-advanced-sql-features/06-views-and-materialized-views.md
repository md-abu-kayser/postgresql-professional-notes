# Views & Materialized Views

```sql
CREATE VIEW active_users AS SELECT * FROM users WHERE active = true;
CREATE MATERIALIZED VIEW user_stats AS SELECT ...
REFRESH MATERIALIZED VIEW CONCURRENTLY user_stats;
```
