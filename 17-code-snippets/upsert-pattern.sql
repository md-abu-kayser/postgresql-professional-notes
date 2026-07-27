INSERT INTO users (email, name, login_count) VALUES ('user@example.com', 'User', 1)
ON CONFLICT (email) DO UPDATE SET name = EXCLUDED.name, login_count = users.login_count + 1;