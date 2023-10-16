import torch.distributed as dist
import torch.multiprocessing as mp
import torch
import os

def ddp_setup():
    dist.init_process_group(backend="nccl")
    torch.cuda.set_device(int(os.environ["LOCAL_RANK"]))


def run():
    local_rank  = int(os.environ["LOCAL_RANK"])
    global_rank = int(os.environ["RANK"])
    world_size = dist.get_world_size()
    torch.manual_seed(global_rank)
    n = torch.randint(high=10, size=(1,), dtype=int).to(local_rank)
    a = torch.tensor([global_rank] * n, dtype=int).to(local_rank)
    for p in range(world_size):
        if global_rank==p:
            print(f"A) {global_rank}: {a}", flush=True)
        dist.barrier()
    nelements_list = [torch.zeros_like(n).to(local_rank) for _ in range(world_size)]
    dist.all_gather(tensor = n, tensor_list = nelements_list)
    gather_list = [torch.zeros(int(nelements_list[i]), dtype=int).to(local_rank) for i in range(world_size)]
    dist.all_gather(tensor = a, tensor_list = gather_list)
    res = torch.cat((gather_list))
    for p in range(world_size):
        if global_rank==p:
            print(f"B) {global_rank}: {res}", flush=True)
        dist.barrier()

def main():
   ddp_setup()
   run()
   dist.destroy_process_group()

if __name__ == "__main__":
##   world_size = torch.cuda.device_count()
##   mp.spawn(main, args=(world_size,), nprocs=world_size)
    main()

