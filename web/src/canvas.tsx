import { Card, CardContent } from '@mui/material';
import Slider from '@mui/material/Slider';
import { useEffect, useRef, useState } from 'react'


declare global {
    interface Window {
        Module: any;
    }
}

interface Props {
    onTick: (tick: number) => void;
    onSimStats: (stats: any) => void;
    onEntityStats: (entityStats: any) => void;
    onSelectedEntity: (selectedEntity: boolean) => void;
    hasSelectedEntity: boolean;
}



function SimulationCanvas({onTick, onSimStats, onEntityStats, onSelectedEntity, hasSelectedEntity}: Props) {
    const canvasRef = useRef<HTMLCanvasElement>(null);
    const [speed, setSpeed] = useState(1);
    const speedRef = useRef(1);
    const engineRef = useRef<any>(null);
    const CELL = 20;
    const hasSelectedEntityRef = useRef(false);

    const handleSpeedChange = (_event: Event, newValue: number) => {
        setSpeed(newValue);
        speedRef.current = newValue;
    };

    const handleCanvasClick = (e: React.MouseEvent<HTMLCanvasElement>) => {
        const rect = canvasRef.current!.getBoundingClientRect();
        const x = Math.floor((e.clientX - rect.left) / CELL);
        const y = Math.floor((e.clientY - rect.top) / CELL);
        const data = engineRef.current.getEntityAt(x, y);
        if (data.exists) {
            hasSelectedEntity
            onSelectedEntity(true);
            hasSelectedEntityRef.current = true;
            onEntityStats({... data});

    }
};

    useEffect(() => {
        const script = document.createElement('script');
        script.src = '/agent-simulation.js';
        script.async = true;
        script.onload = () => {
            window.Module.onRuntimeInitialized = () => {
                const width = 25;
                const height = 25;
                engineRef.current = new window.Module.Engine(12345, 100, width, height);
                const canvas = canvasRef.current;
                if (!canvas) return;
                const ctx = canvas.getContext('2d');
                if (!ctx) return;

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
                        engineRef.current.simulate();
                    }
                    const snapshot = engineRef.current.getGridSnapshot();
                    for (let i = 0; i < snapshot.size(); i++) {
                        const x = (i % width) * CELL;
                        const y = Math.floor(i / height) * CELL;
                        ctx!.fillStyle = COLORS[snapshot.get(i)] ?? '#000';
                        ctx!.fillRect(x, y, CELL, CELL);
                    }
                    snapshot.delete();
                    onTick(engineRef.current.getTick());
                    onSimStats({... engineRef.current.getStatistics()});
                    if(hasSelectedEntityRef.current) {
                        onEntityStats({... engineRef.current.getSelectedEntity()});
                    }
                    requestAnimationFrame(render);
                }

                requestAnimationFrame(render);
            };
        };
        document.body.appendChild(script);

        return () => { document.body.removeChild(script); };
    }, []);

    

    return <Card>
        <CardContent sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', flexDirection: 'column' }}>
            <canvas ref={canvasRef} onClick={handleCanvasClick}/>
            <div>{speed}</div>
            <Slider aria-label="Volume" value={speed} onChange={handleSpeedChange} min={0} max={100} />
        </CardContent>
    </Card> 
}

export default SimulationCanvas;