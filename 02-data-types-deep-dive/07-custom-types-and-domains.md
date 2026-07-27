# Custom Types & Domains

```sql
CREATE DOMAIN email AS TEXT CHECK (VALUE ~ '^[^@]+@[^@]+$');
```
