# Table Partitioning

```sql
CREATE TABLE orders (...) PARTITION BY RANGE (order_date);
CREATE TABLE orders_2024 PARTITION OF orders FOR VALUES FROM ('2024-01-01') TO ('2025-01-01');
```
