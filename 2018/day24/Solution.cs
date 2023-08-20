using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Solution {
    public static void Main(string[] args) {
        long boost = 0;
        while (true) {
            if (Solve(boost++)) {
                break;
            }
        }
    }

    public enum Type {
        FIRE = 0,
        RADIATION = 1,
        COLD = 2,
        BLUDGEONING = 3,
        SLASHING = 4
    }

    static Type FIRE = Type.FIRE;
    static Type RADIATION = Type.RADIATION;
    static Type COLD = Type.COLD;
    static Type BLUDGEONING = Type.BLUDGEONING;
    static Type SLASHING = Type.SLASHING;

    static bool Solve(long boost) {
        // Immune system groups
        HashSet<Group> immuneSystemGroups = new HashSet<Group>();
        // immuneSystemGroups.Add(new Group(17, 5390, Set(), Set(RADIATION, BLUDGEONING), 4507 + boost, FIRE, 2));
        // immuneSystemGroups.Add(new Group(989, 1274, Set(FIRE), Set(BLUDGEONING, SLASHING), 25 + boost, SLASHING, 3));

        immuneSystemGroups.Add(new Group(585, 7536, Set(FIRE, RADIATION, COLD), Set(BLUDGEONING), 116 + boost, RADIATION, 3));
        immuneSystemGroups.Add(new Group(884, 7258, Set(FIRE, BLUDGEONING), Set(), 60 + boost, COLD, 16));
        immuneSystemGroups.Add(new Group(1043, 6844, Set(), Set(FIRE), 56 + boost, BLUDGEONING, 10));
        immuneSystemGroups.Add(new Group(3247, 8560, Set(RADIATION), Set(), 25 + boost, BLUDGEONING, 13));
        immuneSystemGroups.Add(new Group(1898, 4940, Set(), Set(), 25 + boost, SLASHING, 19));
        immuneSystemGroups.Add(new Group(6391, 7766, Set(), Set(FIRE), 11 + boost, BLUDGEONING, 15));
        immuneSystemGroups.Add(new Group(3213, 7310, Set(SLASHING), Set(RADIATION, COLD), 22 + boost, FIRE, 17));
        immuneSystemGroups.Add(new Group(3338, 4820, Set(), Set(FIRE, BLUDGEONING), 14 + boost, COLD, 11));
        immuneSystemGroups.Add(new Group(1099, 4220, Set(FIRE, BLUDGEONING, SLASHING), Set(), 35 + boost, RADIATION, 8));
        immuneSystemGroups.Add(new Group(8629, 1865, Set(), Set(RADIATION), 1 + boost, BLUDGEONING, 20));


        // Infection groups
        HashSet<Group> infectionGroups = new HashSet<Group>();
        // infectionGroups.Add(new Group(801, 4706, Set(), Set(RADIATION), 116, BLUDGEONING, 1));
        // infectionGroups.Add(new Group(4485, 2961, Set(RADIATION), Set(FIRE, COLD), 12, SLASHING, 4));

        infectionGroups.Add(new Group(4401, 9823, Set(), Set(BLUDGEONING), 3, RADIATION, 6));
        infectionGroups.Add(new Group(1783, 55041, Set(), Set(COLD, SLASHING), 45, FIRE, 12));
        infectionGroups.Add(new Group(283, 37907, Set(), Set(RADIATION, BLUDGEONING), 213, BLUDGEONING, 1));
        infectionGroups.Add(new Group(4126, 53542, Set(), Set(), 21, SLASHING, 7));
        infectionGroups.Add(new Group(2362, 31395, Set(FIRE, BLUDGEONING, SLASHING), Set(), 25, RADIATION, 9));
        infectionGroups.Add(new Group(4451, 57776, Set(FIRE), Set(RADIATION), 21, SLASHING, 4));
        infectionGroups.Add(new Group(149, 50075, Set(), Set(COLD, BLUDGEONING), 642, FIRE, 18));
        infectionGroups.Add(new Group(3315, 28807, Set(BLUDGEONING), Set(FIRE), 16, FIRE, 2));
        infectionGroups.Add(new Group(4369, 5279, Set(), Set(), 2, FIRE, 5));
        infectionGroups.Add(new Group(3069, 7553, Set(), Set(), 3, COLD, 14));


        HashSet<Group> dead = new HashSet<Group>();
        while (immuneSystemGroups.Count > 0 && infectionGroups.Count > 0) {
            // Console.WriteLine();

            // Target Selection
            List<Group> targetSelectionOrder = new List<Group>();
            targetSelectionOrder.AddRange(immuneSystemGroups);
            targetSelectionOrder.AddRange(infectionGroups);

            targetSelectionOrder.Sort((x, y) => {
                long xPower = x.GetEffectivePower();
                long yPower = y.GetEffectivePower();

                if (xPower == yPower) {
                    return (int)(y.initiative - x.initiative);
                }
                return yPower.CompareTo(xPower);
            });

            // Now go in order and select group to fight
            Dictionary<Group, Group> attackerToAttacked = new Dictionary<Group, Group>();
            for (int i = 0; i < targetSelectionOrder.Count; i++) {
                Group group = targetSelectionOrder[i];

                HashSet<Group> enemies = immuneSystemGroups.Contains(group) ? infectionGroups : immuneSystemGroups;
                
                Group toAttack = null;
                long bestDamage = 0;
                foreach (Group enemyGroup in enemies) {
                    if (!attackerToAttacked.Values.Contains(enemyGroup)) {
                        long damage = group.GetDamageTotal(enemyGroup);

                        if (damage > bestDamage) {
                            toAttack = enemyGroup;
                            bestDamage = damage;
                        } else if (damage == bestDamage && toAttack != null) {
                            // Damage tie; check effective power
                            long toAttackPower = toAttack.GetEffectivePower();
                            long enemyPower = enemyGroup.GetEffectivePower();
                            if (enemyPower > toAttackPower) {
                                toAttack = enemyGroup;
                                bestDamage = damage;
                            } else if (enemyPower == toAttackPower && enemyGroup.initiative > toAttack.initiative) {
                                // Effective power tie; pick the enemy with higher initiative
                                toAttack = enemyGroup;
                                bestDamage = damage;
                            }
                        }
                    }
                }

                attackerToAttacked.Add(group, toAttack);
            }

            // Attack phase
            targetSelectionOrder.Sort((x, y) => {
                return (int)(y.initiative - x.initiative);
            });

            bool someUnitDied = false;
            for (int i = 0; i < targetSelectionOrder.Count; i++) {
                if (dead.Contains(targetSelectionOrder[i])) {
                    continue;
                }

                Group attacker = targetSelectionOrder[i];
                Group toAttack = attackerToAttacked[attacker];

                if (toAttack == null) {
                    continue;
                }

                bool unitWasKilled = attacker.Attack(toAttack);
                if (unitWasKilled) {
                    someUnitDied = true;
                }

                if (toAttack.units <= 0) {
                    dead.Add(toAttack);
                    immuneSystemGroups.Remove(toAttack);
                    infectionGroups.Remove(toAttack);
                }
            }

            if (!someUnitDied) {
                // If no one dies at end of attack phase, we will loop forever, so break
                break;
            }
        }

        if (boost == 0) {
            Console.WriteLine($"Leftover immune (boost 0): {immuneSystemGroups.Sum(x => x.units)}");
            Console.WriteLine($"Leftover infection (boost 0): {infectionGroups.Sum(x => x.units)}");
        }

        if (immuneSystemGroups.Count > 0 && infectionGroups.Count == 0) {
            Console.WriteLine($"Survived with boost {boost}, num units left is {immuneSystemGroups.Sum(x => x.units)}");
            return true;
        }
        return false;
    }
    
    static HashSet<int> Set(params Type[] items) {
        HashSet<int> set = new HashSet<int>();
        foreach (Type i in items) {
            set.Add((int)i);
        }
        return set;
    }

    private class Group {
        private static int totalId = 0;

        public long units;
        public long unitHp;
        public HashSet<int> immunities;
        public HashSet<int> weaknesses;
        public long unitAttackPower;
        public int attackType;
        public int initiative;

        public int id;

        public Group(long units, long unitHp, HashSet<int> immunities, HashSet<int> weaknesses, long unitAttackPower, Type attackType, int initiative) {
            this.units = units;
            this.unitHp = unitHp;
            this.immunities = immunities;
            this.weaknesses = weaknesses;
            this.unitAttackPower = unitAttackPower;
            this.attackType = (int)attackType;
            this.initiative = initiative;

            this.id = totalId++;
        }

        public long GetEffectivePower() {
            return this.units * this.unitAttackPower;
        }

        public long GetDamageTotal(Group g) {
            long factor = 1;
            if (g.immunities.Contains(this.attackType)) {
                factor = 0;
            } else if (g.weaknesses.Contains(this.attackType)) {
                factor = 2;
            }
            return factor * this.GetEffectivePower();
        }

        public bool Attack(Group g) {
            long damageTotal = this.GetDamageTotal(g);
            g.units -= (damageTotal / g.unitHp);

            // Return true if a unit got killed
            return damageTotal / g.unitHp > 0;
        }

        public override bool Equals(object obj) {
            Group g = (Group)obj;
            return this.id == g.id;
        }

        public override int GetHashCode() {
            return id.GetHashCode();
        }
    }
}