#include "minctest.h"
#include "camp.h"
#include "narrative.h"
#include "probability.h"

// void printJobs(std_vector<uint16_t *> in_jobs) {
//     for (int job = 0; job < in_jobs); job++) {
//         SDL_Log("Job: %s", jobNames[job].c_str());

//         for (int worker = 0; worker < in_jobs[job]); worker++) {
//             SDL_Log("Worker: %s", unitNames[in_jobs[job][worker]].c_str());
//         }
//     }
// }

void test_camp() {
    SDL_Log("test_camp");
    utilities_allNames_Load();
    uint16_t * in_party1 = NULL;
    arrput(in_party1, UNIT_NAME_ERWIN);
    arrput(in_party1, UNIT_NAME_KIARA);
    uint16_t * in_party2 = NULL;
    arrput(in_party2, UNIT_NAME_ERWIN);
    arrput(in_party2, UNIT_NAME_KIARA);
    arrput(in_party2, UNIT_NAME_SILOU);
    arrput(in_party2, UNIT_NAME_PIROU);
    arrput(in_party2, UNIT_NAME_SERVIL);
    arrput(in_party2, UNIT_NAME_PERIGNON);
    arrput(in_party2, UNIT_NAME_LAUZON);
    arrput(in_party2, UNIT_NAME_NICOLE);
    arrput(in_party2, UNIT_NAME_OTTO);
    arrput(in_party2, UNIT_NAME_SEBASTIAN);
    arrput(in_party2, UNIT_NAME_MICHAEL);
    arrput(in_party2, UNIT_NAME_CHASSE);
    arrput(in_party2, UNIT_NAME_PIERRE);
    arrput(in_party2, UNIT_NAME_MELLY);
    arrput(in_party2, UNIT_NAME_RAYAN);
    arrput(in_party2, UNIT_NAME_SIMON);
    arrput(in_party2, UNIT_NAME_EUGENE);
    arrput(in_party2, UNIT_NAME_TEHARON);
    arrput(in_party2, UNIT_NAME_KAKWI);
    arrput(in_party2, UNIT_NAME_LUCRECE);
    int8_t * previous_job = NULL;
    int8_t * priority_job = NULL;
    int8_t * forbidden_job = NULL;
    init_tinyMT(&tinyMT_global);
    for (short int i = 0; i < UNIT_NAME_PC_END; i++) {
        arrput(previous_job, -1);
        arrput(priority_job, -1);
        arrput(forbidden_job, -1);
    }
    struct Camp test_camp1 = Camp_default;
    struct Camp test_camp2 = Camp_default;
    struct Camp test_camp3 = Camp_default;
    struct Camp test_camp4 = Camp_default;
    for (uint8_t i; i < arrlen(in_party1); i++) {
        arrput(test_camp1.party, in_party1[i]);
    }
    for (uint8_t i; i < arrlen(in_party2); i++) {
        arrput(test_camp2.party, in_party2[i]);
    }
    for (uint8_t i; i < arrlen(in_party2); i++) {
        arrput(test_camp3.party, in_party2[i]);
    }
    lok(arrlen(test_camp1.party) == arrlen(in_party1));
    if (arrlen(test_camp1.party) == arrlen(in_party1)) {
        for (uint8_t i; i < arrlen(test_camp1.party); i++) {
            lok(test_camp1.party[i] == in_party1[i]);
        }
    }
    int8_t * out_priorityjobs;
    Camp_JobQueue_Compute(&test_camp1);
    uint8_t * out_queue = test_camp1.job_queue;
    lok(out_queue[0] == CAMPJOB_COOK);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_STABLEHAND);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_GUARD);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_STORAGEMASTER);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_CLERGYMAN);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_LIBRARIAN);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_SCRIBE);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_ASSISTANT);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_MAGICRESEARCH);
    arrdel(out_queue, 0);
    uint8_t * out_optimal;
    Camp_Jobs_Compute(&test_camp1);
    out_optimal = test_camp1.optimal_jobs;
    lok(out_optimal[CAMPJOB_LIBRARIAN] == 0);
    lok(out_optimal[CAMPJOB_COOK] == 1);
    lok(out_optimal[CAMPJOB_GUARD] == 1);
    lok(out_optimal[CAMPJOB_SCRIBE] == 0);
    lok(out_optimal[CAMPJOB_STABLEHAND] == 1);
    lok(out_optimal[CAMPJOB_ASSISTANT] == 0);
    lok(out_optimal[CAMPJOB_CLERGYMAN] == 1);
    lok(out_optimal[CAMPJOB_STORAGEMASTER] == 0);
    lok(out_optimal[CAMPJOB_MAGICRESEARCH] == 0);
    Camp_Jobs_Clear(&test_camp1);
    Camp_JobQueue_Compute(&test_camp1);
    Camp_partyStack_Make(&test_camp1);
    Camp_Jobs_Assign(&test_camp1);
    uint16_t ** out_jobs;
    out_jobs = test_camp1.jobs;
    lok(arrlen(out_jobs[CAMPJOB_LIBRARIAN]) == 0);
    lok(arrlen(out_jobs[CAMPJOB_COOK]) == 1); //  Kiara
    lok(arrlen(out_jobs[CAMPJOB_GUARD]) == 0);
    lok(arrlen(out_jobs[CAMPJOB_SCRIBE]) == 0);
    lok(arrlen(out_jobs[CAMPJOB_STABLEHAND]) == 1); // Erwin
    lok(arrlen(out_jobs[CAMPJOB_ASSISTANT]) == 0);
    lok(arrlen(out_jobs[CAMPJOB_CLERGYMAN]) == 0);
    lok(arrlen(out_jobs[CAMPJOB_STORAGEMASTER]) == 0);
    lok(arrlen(out_jobs[CAMPJOB_MAGICRESEARCH]) == 0);
    lok(arrlen(test_camp2.party) == arrlen(in_party2));
    if (arrlen(test_camp2.party) == arrlen(in_party2)) {
        for (uint8_t i; i < arrlen(test_camp2.party); i++) {
            lok(test_camp2.party[i] == in_party2[i]);
        }
    }
    Camp_Jobs_Clear(&test_camp2);
    Camp_JobQueue_Compute(&test_camp2);
    out_queue = test_camp2.job_queue;
    lok(out_queue[0] == CAMPJOB_COOK);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_STABLEHAND);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_GUARD);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_STORAGEMASTER);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_CLERGYMAN);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_LIBRARIAN);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_SCRIBE);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_ASSISTANT);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_MAGICRESEARCH);
    Camp_Jobs_Compute(&test_camp2);
    out_optimal = test_camp2.optimal_jobs;
    lok(out_optimal[CAMPJOB_LIBRARIAN] == 1);
    lok(out_optimal[CAMPJOB_COOK] == 1);
    lok(out_optimal[CAMPJOB_GUARD] == 3);
    lok(out_optimal[CAMPJOB_SCRIBE] == 0);
    lok(out_optimal[CAMPJOB_STABLEHAND] == 1);
    lok(out_optimal[CAMPJOB_ASSISTANT] == 0);
    lok(out_optimal[CAMPJOB_CLERGYMAN] == 2);
    lok(out_optimal[CAMPJOB_STORAGEMASTER] == 1);
    lok(out_optimal[CAMPJOB_MAGICRESEARCH] == 0);
    Camp_Jobs_Clear(&test_camp2);
    Camp_JobQueue_Compute(&test_camp2);
    Camp_partyStack_Make(&test_camp2);
    lok(arrlen(test_camp2.party_stack) == (arrlen(in_party2)));
    Camp_Jobs_Assign(&test_camp2);
    out_jobs = test_camp2.jobs;
    out_optimal = test_camp2.optimal_jobs;
    lok(arrlen(out_jobs[CAMPJOB_LIBRARIAN]) == out_optimal[CAMPJOB_LIBRARIAN]);
    lok(arrlen(out_jobs[CAMPJOB_COOK]) == out_optimal[CAMPJOB_COOK]);
    lok(arrlen(out_jobs[CAMPJOB_GUARD]) == out_optimal[CAMPJOB_GUARD]);
    lok(arrlen(out_jobs[CAMPJOB_SCRIBE]) == out_optimal[CAMPJOB_SCRIBE]);
    lok(arrlen(out_jobs[CAMPJOB_STABLEHAND]) == out_optimal[CAMPJOB_STABLEHAND]);
    lok(arrlen(out_jobs[CAMPJOB_ASSISTANT]) == out_optimal[CAMPJOB_ASSISTANT]);
    lok(arrlen(out_jobs[CAMPJOB_CLERGYMAN]) == out_optimal[CAMPJOB_CLERGYMAN]);
    lok(arrlen(out_jobs[CAMPJOB_STORAGEMASTER]) == out_optimal[CAMPJOB_STORAGEMASTER]);
    lok(arrlen(out_jobs[CAMPJOB_MAGICRESEARCH]) == out_optimal[CAMPJOB_MAGICRESEARCH]);
    lok(out_jobs[CAMPJOB_LIBRARIAN][0] == UNIT_NAME_MELLY);
    lok(out_jobs[CAMPJOB_COOK][0] == UNIT_NAME_LUCRECE);
    lok(out_jobs[CAMPJOB_GUARD][0] == UNIT_NAME_EUGENE);
    lok(out_jobs[CAMPJOB_GUARD][1] == UNIT_NAME_MICHAEL);
    lok(out_jobs[CAMPJOB_GUARD][2] == UNIT_NAME_LAUZON);
    lok(out_jobs[CAMPJOB_STABLEHAND][0] == UNIT_NAME_KAKWI);
    lok(out_jobs[CAMPJOB_CLERGYMAN][0] == UNIT_NAME_RAYAN);
    lok(out_jobs[CAMPJOB_CLERGYMAN][1] == UNIT_NAME_NICOLE);
    lok(out_jobs[CAMPJOB_STORAGEMASTER][0] == UNIT_NAME_SIMON);
    test_camp3.forbidden_jobs[UNIT_NAME_NICOLE] = CAMPJOB_LIBRARIAN;
    test_camp3.priority_jobs[UNIT_NAME_KIARA] = CAMPJOB_CLERGYMAN;
    Camp_JobQueue_Compute(&test_camp3);
    out_queue = test_camp3.job_queue;
    lok(out_queue[0] == CAMPJOB_COOK);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_STABLEHAND);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_GUARD);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_STORAGEMASTER);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_CLERGYMAN);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_LIBRARIAN);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_SCRIBE);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_ASSISTANT);
    arrdel(out_queue, 0);
    lok(out_queue[0] == CAMPJOB_MAGICRESEARCH);
    Camp_Jobs_Compute(&test_camp3);
    out_optimal = test_camp3.optimal_jobs;
    lok(out_optimal[CAMPJOB_LIBRARIAN] == 1);
    lok(out_optimal[CAMPJOB_COOK] == 1);
    lok(out_optimal[CAMPJOB_GUARD] == 3);
    lok(out_optimal[CAMPJOB_SCRIBE] == 0);
    lok(out_optimal[CAMPJOB_STABLEHAND] == 1);
    lok(out_optimal[CAMPJOB_ASSISTANT] == 0);
    lok(out_optimal[CAMPJOB_CLERGYMAN] == 2);
    lok(out_optimal[CAMPJOB_STORAGEMASTER] == 1);
    lok(out_optimal[CAMPJOB_MAGICRESEARCH] == 0);
    Camp_Jobs_Clear(&test_camp3);
    Camp_partyStack_Make(&test_camp3);
    lok(arrlen(test_camp3.party_stack) == (arrlen(in_party2) - 1));
//     // -1 for kiara. priority jobs don't get added in the stack.
    Camp_Jobs_Clear(&test_camp3);
    Camp_JobQueue_Compute(&test_camp3);
    Camp_partyStack_Make(&test_camp3);
    Camp_Jobs_Assign(&test_camp3);
    out_jobs = test_camp3.jobs;
    out_optimal = test_camp3.optimal_jobs;
    lok(arrlen(out_jobs[CAMPJOB_LIBRARIAN]) == out_optimal[CAMPJOB_LIBRARIAN]);
    lok(arrlen(out_jobs[CAMPJOB_COOK]) == out_optimal[CAMPJOB_COOK]);
    lok(arrlen(out_jobs[CAMPJOB_GUARD]) == out_optimal[CAMPJOB_GUARD]);
    lok(arrlen(out_jobs[CAMPJOB_SCRIBE]) == out_optimal[CAMPJOB_SCRIBE]);
    lok(arrlen(out_jobs[CAMPJOB_STABLEHAND]) == out_optimal[CAMPJOB_STABLEHAND]);
    lok(arrlen(out_jobs[CAMPJOB_ASSISTANT]) == out_optimal[CAMPJOB_ASSISTANT]);
    lok(arrlen(out_jobs[CAMPJOB_CLERGYMAN]) == out_optimal[CAMPJOB_CLERGYMAN]);
    lok(arrlen(out_jobs[CAMPJOB_STORAGEMASTER]) == out_optimal[CAMPJOB_STORAGEMASTER]);
    lok(arrlen(out_jobs[CAMPJOB_MAGICRESEARCH]) == out_optimal[CAMPJOB_MAGICRESEARCH]);
    lok(out_jobs[CAMPJOB_LIBRARIAN][0] == UNIT_NAME_ERWIN);
    lok(out_jobs[CAMPJOB_COOK][0] == UNIT_NAME_MELLY);
    lok(out_jobs[CAMPJOB_GUARD][0] == UNIT_NAME_CHASSE);
    lok(out_jobs[CAMPJOB_GUARD][1] == UNIT_NAME_SILOU);
    lok(out_jobs[CAMPJOB_GUARD][2] == UNIT_NAME_PIROU);
    lok(out_jobs[CAMPJOB_STABLEHAND][0] == UNIT_NAME_PIERRE);
    lok(out_jobs[CAMPJOB_CLERGYMAN][0] == UNIT_NAME_KIARA); // piorityjob
    lok(out_jobs[CAMPJOB_CLERGYMAN][1] == UNIT_NAME_LAUZON);
    lok(out_jobs[CAMPJOB_STORAGEMASTER][0] == UNIT_NAME_MICHAEL);
    Camp_Jobs_Clear(&test_camp3);
    arrput(test_camp3.exclusions, UNIT_NAME_SILOU);
    Camp_partyStack_Make(&test_camp3);
//     SDL_Log("party stack size: %d", test_camp3.getPartyStack()));
    lok(arrlen(test_camp3.party_stack) == (arrlen(in_party2) - 2));
//     // -3 for kiara AND erwin AND exclusion. priority jobs don't get added in the stack.
    Camp_Jobs_Clear(&test_camp3);
    Camp_partyStack_Make(&test_camp3);
    Camp_Jobs_Assign(&test_camp3);
    out_jobs = test_camp3.jobs;
    lok(arrlen(out_jobs[CAMPJOB_LIBRARIAN]) == out_optimal[CAMPJOB_LIBRARIAN]);
    lok(arrlen(out_jobs[CAMPJOB_COOK]) == out_optimal[CAMPJOB_COOK]);
    lok(arrlen(out_jobs[CAMPJOB_GUARD]) == out_optimal[CAMPJOB_GUARD]);
    lok(arrlen(out_jobs[CAMPJOB_SCRIBE]) == out_optimal[CAMPJOB_SCRIBE]);
    lok(arrlen(out_jobs[CAMPJOB_STABLEHAND]) == out_optimal[CAMPJOB_STABLEHAND]);
    lok(arrlen(out_jobs[CAMPJOB_ASSISTANT]) == out_optimal[CAMPJOB_ASSISTANT]);
    lok(arrlen(out_jobs[CAMPJOB_CLERGYMAN]) == out_optimal[CAMPJOB_CLERGYMAN]);
    lok(arrlen(out_jobs[CAMPJOB_STORAGEMASTER]) == out_optimal[CAMPJOB_STORAGEMASTER]);
    lok(out_jobs[CAMPJOB_LIBRARIAN][0] == UNIT_NAME_PIERRE);
    lok(out_jobs[CAMPJOB_COOK][0] == UNIT_NAME_CHASSE);
    lok(out_jobs[CAMPJOB_GUARD][0] == UNIT_NAME_KAKWI);
    lok(out_jobs[CAMPJOB_GUARD][1] == UNIT_NAME_OTTO);
    lok(out_jobs[CAMPJOB_GUARD][2] == UNIT_NAME_LAUZON);
    lok(out_jobs[CAMPJOB_STABLEHAND][0] == UNIT_NAME_MICHAEL);
    lok(out_jobs[CAMPJOB_CLERGYMAN][0] == UNIT_NAME_KIARA);
    lok(out_jobs[CAMPJOB_CLERGYMAN][1] == UNIT_NAME_SIMON);
    lok(out_jobs[CAMPJOB_STORAGEMASTER][0] == UNIT_NAME_EUGENE);
    writeJSON("saves//camp_test.json", &test_camp3, false);
    lok(PHYSFS_exists("saves//camp_test.json") > 0);
    readJSON("saves//camp_test.json", &test_camp4);
    writeJSON("saves//camp_rewrite.json", &test_camp4, false);
    lok(PHYSFS_exists("saves//camp_rewrite.json") > 0);
    lok(fequal("saves//camp_rewrite.json", "saves//camp_test.json"));
}
