# PL/pgSQL Basics

```sql
CREATE FUNCTION greet(name TEXT) RETURNS TEXT AS $$
BEGIN
  RETURN 'Hello, ' || name || '!';
END;
$$ LANGUAGE plpgsql;
```
