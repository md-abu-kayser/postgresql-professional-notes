-- Offset pagination (simple but inefficient for large offsets)
SELECT * FROM users ORDER BY id LIMIT 20 OFFSET 40;
-- Keyset pagination (more efficient)
SELECT * FROM users WHERE id > 40 ORDER BY id LIMIT 20;