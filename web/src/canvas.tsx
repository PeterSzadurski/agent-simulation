import Slider from '@mui/material/Slider';
import { useEffect, useRef, useState } from 'react'

declare global {
    interface Window {
        Module: any;
    }
}

function SimulationCanvas() {
    const canvasRef = useRef<HTMLCanvasElement>(null);
    const [tick, setTick] = useState(0);
    const [speed, setSpeed] = useState(1);
    const speedRef = useRef(1);

    const handleSpeedChange = (event: Event, newValue: number) => {
        setSpeed(newValue);
        speedRef.current = newValue;
    };
    
    useEffect(() => {
        const script = document.createElement('script');
        script.src = '/agent-simulation.js';
        script.async = true;
        script.onload = () => {
            window.Module.onRuntimeInitialized = () => {
                const width = 25;
                const height = 25;
                const engine = new window.Module.Engine(12345, 100, width, height);
                const canvas = canvasRef.current;
                if (!canvas) return;
                const ctx = canvas.getContext('2d');
                if (!ctx) return;

                const CELL = 20;
                canvas.width = width * CELL;
                canvas.height = height * CELL;

                const COLORS: Record<number, string> = {
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
                    for (let n = 0; n < speedRef.current; n++ ){
                        engine.simulate();
                    }
                    const snapshot = engine.getGridSnapshot();
                    for (let i = 0; i < snapshot.size(); i++) {
                        const x = (i % width) * CELL;
                        const y = Math.floor(i / height) * CELL;
                        ctx!.fillStyle = COLORS[snapshot.get(i)] ?? '#000';
                        ctx!.fillRect(x, y, CELL, CELL);
                    }
                    snapshot.delete();
                    setTick(engine.getTick());
                    requestAnimationFrame(render);
                }

                requestAnimationFrame(render);
            };
        };
        document.body.appendChild(script);

        return () => { document.body.removeChild(script); };
    }, []);

    return <>
    <canvas ref={canvasRef} />
    <div>{tick}</div>
    <div>{speed}</div>
    <Slider aria-label="Volume" value={speed} onChange={handleSpeedChange} min={0} max={100} />
    </> 
}

export default SimulationCanvas;