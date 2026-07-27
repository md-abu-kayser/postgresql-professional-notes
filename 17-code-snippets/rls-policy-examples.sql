ALTER TABLE tasks ENABLE ROW LEVEL SECURITY;
CREATE POLICY task_user_policy ON tasks FOR ALL USING (user_id = current_setting('app.current_user_id')::int);