import { Accordion, AccordionDetails, AccordionSummary, Card, CardContent, Divider, Typography } from "@mui/material";
import ArrowDropDownIcon from '@mui/icons-material/ArrowDropDown';
import EntityStats from "./EntityStats";
interface Props {
    entityStats: EntityStats;
}

const ENTITY_NAMES: Record<number, string> = {
    0: 'Empty',
    1: 'NPC',
    2: 'Meal',
    3: 'Grass',
    4: 'Deer',
    5: 'Campfire',
    6: 'Raw Meat',
    7: 'Tree',
    8: 'Wood',
};

const STATE_NAMES: Record<number, string> = {
    0: 'Idle',
    1: 'Wander',
    2: 'Walking To',
    3: 'Fleeing From',
};

function EntityStatsPanel({entityStats}: Props) {
    if (!entityStats.exists) {
        <></>
    }
    return(
    <Card>
        <Typography variant="h6">Entity Stats</Typography>
        <Divider sx={{ mb: 1 }} />
        <Typography>Id: {entityStats.id}</Typography>
        <Typography>Is Alive?: {String(entityStats.alive)}</Typography>
        <Typography>Type: {ENTITY_NAMES[entityStats.type]}</Typography>
        <Divider sx={{ mb: 1 }} />
        <Typography>Position ({entityStats.x}, {entityStats.y})</Typography>
        {entityStats.hasHunger?<>
            <Divider sx={{ mb: 1 }} />
            <Typography>Hunger: {entityStats.hunger}</Typography>
        </>
        : <></>}
        {entityStats.hasFuel?<>
            <Divider sx={{ mb: 1 }} />
            <Typography>Fuel: {entityStats.fuel}</Typography>
        </>
        : <></>}
            {entityStats.hasInventory? <Accordion>
            <AccordionSummary expandIcon={<ArrowDropDownIcon/>}>
                <Typography component="span">Inventory</Typography>
            </AccordionSummary>
            <AccordionDetails>
                <Typography>Meals: {entityStats.meals}</Typography>
                <Typography>Raw Meat: {entityStats.rawMeat}</Typography>
                <Typography>Wood: {entityStats.wood}</Typography>
            </AccordionDetails>
        </Accordion> : <></>}
        {entityStats.hasState? <Accordion>
            <AccordionSummary expandIcon={<ArrowDropDownIcon/>}>
                <Typography component="span">State</Typography>
            </AccordionSummary>
            <AccordionDetails>
                <Typography>{STATE_NAMES[entityStats.state]}</Typography>
                {entityStats.hasDestination? 
                <Typography>Destination: ({entityStats.destX}, {entityStats.destY})</Typography>: <></>}
            </AccordionDetails>
        </Accordion> : <></>}
        {entityStats.hasKnowledge? <Accordion>
            <AccordionSummary expandIcon={<ArrowDropDownIcon/>}>
                <Typography component="span">Knowledge</Typography>
            </AccordionSummary>
            <AccordionDetails>
                <Typography>Closest Grass: ({entityStats.closestGrassX}, {entityStats.closestGrassY})</Typography>
                <Typography>Closest Raw Meat: ({entityStats.closestFoodX}, {entityStats.closestFoodY})</Typography>
                <Typography>Closest Deer: ({entityStats.closestDeerX}, {entityStats.closestDeerY})</Typography>
                <Typography>Closest Tree: ({entityStats.closestTreeX}, {entityStats.closestTreeY})</Typography>
            </AccordionDetails>
        </Accordion> : <></>}
                {entityStats.hasFeats? <Accordion>
            <AccordionSummary expandIcon={<ArrowDropDownIcon/>}>
                <Typography component="span">Knowledge</Typography>
            </AccordionSummary>
            <AccordionDetails>
                <Typography>Trees Chopped: {entityStats.choppedTrees}</Typography>
                <Typography>Meals Cooked: {entityStats.mealsCooked}</Typography>
                <Typography>Deer Slain: {entityStats.slainDeer}</Typography>
            </AccordionDetails>
        </Accordion> : <></>}

        

    </Card>
    )
}
export default EntityStatsPanel