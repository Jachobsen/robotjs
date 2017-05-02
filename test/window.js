var test = require('tape');
var robot = require('..');

test('Get window list.', function(t)
{
    var windowList;

    t.plan(3);
    t.ok(windowList = robot.getWindowList(), 'got window list');
    t.ok(windowList.length > 0, 'window list has content')
    t.ok(windowList.filter(w => w.ownerIsFocused).length > 0, 'found a focused process');
});
