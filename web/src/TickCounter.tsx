import { Card, CardContent, Typography } from "@mui/material";

interface Props {
    tick: number;
}
function TickCounter({tick}: Props) {
    return (
    <Card>
        <CardContent sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', flexDirection: 'column' }}>
            <Typography variant="h6">Ticks:</Typography>
            <Typography>{tick}</Typography>
        </CardContent>
    </Card>
    )
}
export default TickCounter
