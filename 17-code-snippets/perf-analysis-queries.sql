-- Find top queries by total time
SELECT query, calls, total_time, mean_time FROM pg_stat_statements ORDER BY total_time DESC LIMIT 10;
-- Check for unused indexes
SELECT relname, indexrelname, idx_scan FROM pg_stat_user_indexes WHERE idx_scan = 0;