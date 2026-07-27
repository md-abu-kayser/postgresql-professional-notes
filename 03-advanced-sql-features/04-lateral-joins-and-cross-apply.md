# LATERAL Joins

```sql
SELECT u.name, p.title
FROM users u
JOIN LATERAL (SELECT title FROM posts WHERE author_id = u.id ORDER BY created_at LIMIT 1) p ON true;
```
