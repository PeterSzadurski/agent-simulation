Module.onRuntimeInitialized = () => {
    const engine = new Module.Engine(12345, 25, 25);
    const canvas = document.getElementById('grid');
    const ctx = canvas.getContext('2d');
    const CELL = 20;
    canvas.width = 25 * CELL;
    canvas.height = 25 * CELL;

    const COLORS = {
        0: '#1a1a2e',   // empty
        1: '#69cdfe',   // npc
        2: '#f5a623',   // meal
        3: '#edf37e',   // grass
        4: '#8b4513',   // deer
        5: '#ff6600',   // campfire
        6: '#cc3300',   // raw_meat
        7: '#228b22',   // tree
        8: '#d1784f',   // wood
    };
    function render() {
        console.log("test 3");
        engine.simulate();
        const snapshot = engine.getGridSnapshot();
        for (let i = 0; i < snapshot.size(); i++) {
            console.log("test 3");
            const x = (i % 25) * CELL;
            const y = Math.floor(i / 25) * CELL;
            ctx.fillStyle = COLORS[snapshot.get(i)] ?? '#000';
            ctx.fillRect(x, y, CELL, CELL);
        }
        snapshot.delete();
        requestAnimationFrame(render);
    }

    requestAnimationFrame(render);
};