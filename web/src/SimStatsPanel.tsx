import { Card, CardContent, Divider, Typography } from '@mui/material';
import SimStats from './SimStats';
import { memo } from 'react';

interface Props {
    simStats: SimStats;
}
const SimStatsPanel = memo(function SimStatsPanel({ simStats }: Props) {
    return (
        <Card sx={{ height: '100%' }}>
            <CardContent>
                <Typography variant="h6">Simulation Stats</Typography>
                <Divider sx={{ mb: 1 }} />
                <Typography>Deer Slain: {simStats.totalDeersSlain}</Typography>
                <Typography>Meals Cooked: {simStats.totalMealsCooked}</Typography>
                <Typography>Meals Eaten: {simStats.totalMealsEaten}</Typography>
                <Typography>Trees Chopped: {simStats.totalTreesChopped}</Typography>
            </CardContent>
        </Card>
    );
});
export default SimStatsPanel;
