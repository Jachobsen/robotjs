var test = require('tape');
var robot = require('..');

test('Processes.', function(t)
{
    t.plan(1);
    t.ok(robot.getFocusedProcess(), `got focused process`)
});
