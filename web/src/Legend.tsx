import { Card, CardContent, Typography } from '@mui/material';
import { ENTITY_COLORS } from './Constants';
import { memo } from 'react';

interface ColoredProps {
    entityType: number;
    children: React.ReactNode;
}

const E = ({ entityType, children }: ColoredProps) => (
    <strong style={{ color: ENTITY_COLORS[entityType] }}>{children}</strong>
);

const Legend = memo(function Legend() {
    return (
        <Card>
            <CardContent>
                <Typography variant="h6">Legend:</Typography>
                <Typography>
                    The <E entityType={5}>Campfire</E> is used by NPCs to cook food, store extra
                    meals and write down knowledge.
                </Typography>
                <Typography>
                    <E entityType={1}>NPCs</E> hunt <E entityType={4}>Deer</E> for meat, cut down{' '}
                    <E entityType={7}>Trees </E>
                    to fuel the <E entityType={5}>Campfire</E> in order to cook meals.
                </Typography>
                <Typography>
                    <E entityType={4}>Deer</E> eat <E entityType={3}>Grass</E> and run from{' '}
                    <E entityType={1}>NPCs</E>
                </Typography>
                <Typography>
                    Use the slider to control simulation speed, a speed of 0 will pause the
                    simulation.
                </Typography>
                <Typography>
                    Click/Tap on an entity to view it's stats, inventory, knowledge, hunger, and
                    position in real time.
                </Typography>
                <Typography>
                    For more details, view the{' '}
                    <a target="_blank" href="https://github.com/PeterSzadurski/agent-simulation">
                        readme
                    </a>
                </Typography>
            </CardContent>
        </Card>
    );
});
export default Legend;
