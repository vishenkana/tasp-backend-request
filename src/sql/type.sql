--------------------------------------------------------------------------------
CREATE TYPE requests.stages AS ENUM ('creation', 'ack', 'extract', 'delivery', 'waiting_write', 'write', 'closing');
COMMENT ON TYPE requests.stages IS 'Этапы';

--------------------------------------------------------------------------------
CREATE TYPE requests.stages_status AS ENUM ('pending', 'warning', 'success', 'error');
COMMENT ON TYPE requests.stages_status IS 'Статусы этапов';
