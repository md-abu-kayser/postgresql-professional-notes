# Recursive CTEs

Traverse trees:

```sql
WITH RECURSIVE subordinates AS (
  SELECT id, name, manager_id FROM employees WHERE id = 1
  UNION ALL
  SELECT e.id, e.name, e.manager_id FROM employees e INNER JOIN subordinates s ON e.manager_id = s.id
)
SELECT * FROM subordinates;
```
