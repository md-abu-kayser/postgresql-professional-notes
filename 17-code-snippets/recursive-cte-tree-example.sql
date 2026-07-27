WITH RECURSIVE org_tree AS (
  SELECT id, name, manager_id, 1 AS level FROM employees WHERE manager_id IS NULL
  UNION ALL
  SELECT e.id, e.name, e.manager_id, t.level + 1
  FROM employees e JOIN org_tree t ON e.manager_id = t.id
)
SELECT * FROM org_tree;